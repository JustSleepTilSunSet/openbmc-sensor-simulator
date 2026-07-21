#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <sdbusplus/asio/connection.hpp>
#include <sdbusplus/asio/object_server.hpp>

#include <charconv>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace
{
constexpr const char* serviceName = "xyz.openbmc_project.MockSensor";
constexpr const char* sensorPath =
    "/xyz/openbmc_project/sensors/temperature/arduino_lm75";
constexpr const char* sensorIface = "xyz.openbmc_project.Sensor.Value";
constexpr const char* assocIfaceName =
    "xyz.openbmc_project.Association.Definitions";

constexpr const char* serialDevice = "/dev/ttyS0";
constexpr unsigned int baudRate = 115200;

constexpr double minValue = -55.0;
constexpr double maxValue = 125.0;

using Association =
    std::tuple<std::string, std::string, std::string>;

std::string trim(std::string_view input)
{
    while (!input.empty() &&
           (input.front() == ' ' || input.front() == '\t' ||
            input.front() == '\r' || input.front() == '\n'))
    {
        input.remove_prefix(1);
    }

    while (!input.empty() &&
           (input.back() == ' ' || input.back() == '\t' ||
            input.back() == '\r' || input.back() == '\n'))
    {
        input.remove_suffix(1);
    }

    return std::string(input);
}

std::optional<double> parseLm75Line(const std::string& rawLine)
{
    const std::string line = trim(rawLine);
    constexpr std::string_view prefix = "TEMP_C=";

    if (line.rfind(prefix, 0) != 0)
    {
        return std::nullopt;
    }

    std::string_view numberView(line);
    numberView.remove_prefix(prefix.size());

    if (numberView.empty())
    {
        return std::nullopt;
    }

    double value = 0.0;
    const char* begin = numberView.data();
    const char* end = numberView.data() + numberView.size();

    auto result = std::from_chars(begin, end, value);

    if (result.ec != std::errc{} || result.ptr != end)
    {
        return std::nullopt;
    }

    if (value < minValue || value > maxValue)
    {
        return std::nullopt;
    }

    return value;
}

class ArduinoLm75Reader
{
  public:
    ArduinoLm75Reader(
        boost::asio::io_context& io,
        std::shared_ptr<sdbusplus::asio::dbus_interface> valueIface) :
        serial(io),
        valueIface(std::move(valueIface))
    {
    }

    void start()
    {
        boost::system::error_code ec;

        serial.open(serialDevice, ec);
        if (ec)
        {
            std::cerr << "Cannot open Arduino LM75 serial device "
                      << serialDevice << ": " << ec.message() << '\n';
            std::exit(EXIT_FAILURE);
        }

        serial.set_option(boost::asio::serial_port_base::baud_rate(baudRate));
        serial.set_option(boost::asio::serial_port_base::character_size(8));
        serial.set_option(boost::asio::serial_port_base::parity(
            boost::asio::serial_port_base::parity::none));
        serial.set_option(boost::asio::serial_port_base::stop_bits(
            boost::asio::serial_port_base::stop_bits::one));
        serial.set_option(boost::asio::serial_port_base::flow_control(
            boost::asio::serial_port_base::flow_control::none));

        std::cerr << "Arduino LM75 serial source: " << serialDevice << " @ "
                  << baudRate << '\n';

        readLine();
    }

  private:
    void readLine()
    {
        boost::asio::async_read_until(
            serial, buffer, '\n',
            [this](const boost::system::error_code& ec, std::size_t)
            {
                if (ec)
                {
                    std::cerr << "Arduino LM75 serial read failed from "
                              << serialDevice << ": " << ec.message() << '\n';
                    return;
                }

                std::istream stream(&buffer);
                std::string line;
                std::getline(stream, line);

                const std::optional<double> parsed = parseLm75Line(line);

                if (!parsed.has_value())
                {
                    std::cerr << "Reject invalid LM75 serial line: [" << line
                              << "]\n";
                    readLine();
                    return;
                }

                const double value = parsed.value();

                valueIface->set_property("Value", value);

                std::cerr << "LM75 update: TEMP_C=" << value << '\n';

                readLine();
            });
    }

    boost::asio::serial_port serial;
    boost::asio::streambuf buffer;
    std::shared_ptr<sdbusplus::asio::dbus_interface> valueIface;
};

} // namespace

int main()
{
    boost::asio::io_context io;

    auto systemBus = std::make_shared<sdbusplus::asio::connection>(io);
    systemBus->request_name(serviceName);

    sdbusplus::asio::object_server objectServer(systemBus);

    auto valueIface = objectServer.add_interface(sensorPath, sensorIface);

    double value = 0.0;

    valueIface->register_property("Value", value);
    valueIface->register_property(
        "Unit",
        std::string("xyz.openbmc_project.Sensor.Value.Unit.DegreesC"));
    valueIface->register_property("MinValue", minValue);
    valueIface->register_property("MaxValue", maxValue);

    valueIface->initialize();

    std::vector<Association> associations = {
        {
            "chassis",
            "all_sensors",
            "/xyz/openbmc_project/inventory/system/chassis",
        },
    };

    auto assocIface = objectServer.add_interface(sensorPath, assocIfaceName);
    assocIface->register_property("Associations", associations);
    assocIface->initialize();

    ArduinoLm75Reader reader(io, valueIface);
    reader.start();

    io.run();

    return 0;
}
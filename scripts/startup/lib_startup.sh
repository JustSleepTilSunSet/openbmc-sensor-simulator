sudo apt update

# Needed to compile your C++ app
sudo apt install -y \
  build-essential \
  g++ \
  pkg-config \
  libsystemd-dev \
  libboost-dev \
  unison

# Needed to build/install sdbusplus from source
sudo apt install -y \
  git \
  meson \
  ninja-build \
  python3 \
  python3-yaml \
  python3-mako \
  python3-inflection \
  nlohmann-json3-dev

(https://github.com/uudam42/cpp_sniffer/actions/workflows/cmake.yml/badge.svg)](https://github.com/uudam42/cpp_sniffer/actions)

<!-- C++ 版本 -->
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)

<!-- License -->
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://github.com/uudam42/cpp_sniffer/blob/main/LICENSE)


# C++ Sniffer

A simple command-line network packet sniffer built in C++ using **libpcap**. It supports:

- Real-time packet capture on specified network interface (default: `en0`)
- BPF filtering (e.g., `tcp port 80`, `icmp`)
- Capture count limitation (`-n <count>`) or unlimited until interrupted
- Graceful interruption with `Ctrl-C` (SIGINT) and summary statistics
- Ethernet, IPv4, TCP/UDP header parsing and display
- Optional time-stamped logging to file (`-o <logfile>`)
- Cross-platform build using CMake

---

## Table of Contents

- [Prerequisites](#prerequisites)
- [Directory Structure](#directory-structure)
- [Building](#building)
- [Usage](#usage)
  - [Infinite Capture](#infinite-capture)
  - [Count-limited Capture](#count-limited-capture)
  - [BPF Filter](#bpf-filter)
  - [Logging](#logging)
- [Demo](#demo)
- [Contributing](#contributing)
- [License](#license)

---

## Prerequisites

- **C++17** compatible compiler (e.g., `clang++`, `g++`)
- **CMake** (version ≥ 3.10)
- **libpcap** development library installed
  - macOS: `brew install libpcap`
  - Ubuntu: `sudo apt-get install libpcap-dev`

---

## Directory Structure

```
cpp_sniffer/
├── CMakeLists.txt        # CMake build script
├── src/                  # Source files
│   ├── main.cpp
│   ├── sniffer.cpp
│   ├── parser.cpp
│   └── utils.cpp
├── include/              # Header files
│   ├── sniffer.hpp
│   ├── parser.hpp
│   └── utils.hpp
└── .gitignore            # Ignore build artifacts, logs, etc.

```

---

## Building

1. Create and enter a build directory:

   ```bash
   mkdir -p build && cd build
   ```

2. Configure with CMake:

   ```bash
   cmake -S .. -B .
   ```

3. Compile:

   ```bash
   cmake --build .
   ```

After a successful build, the `sniffer` executable will be in the `build/` directory.

---

## Usage

Run `sniffer` with `sudo` (packet capture requires elevated privileges).

### Infinite Capture

```bash
cd build
sudo ./sniffer
```

Press `Ctrl-C` to stop. You will see a summary:

```
Capture interrupted, total packets: X
```

### Count-limited Capture

Capture exactly `N` packets, then exit:

```bash
sudo ./sniffer -n 5
```

### BPF Filter

Only capture traffic matching a BPF expression (e.g., HTTP port 80):

```bash
sudo ./sniffer -f "tcp port 80" -n 3
```

### Logging

Write output to a timestamped log file:

```bash
sudo ./sniffer -f "tcp port 80" -n 10 -o sniff.log
```

Log file will contain the same output with ISO‑formatted timestamps.

---

## Demo

**Start capturing (unlimited)**

![start_capture](https://github.com/user-attachments/assets/423befa9-c9ff-4c26-90f1-a9f5bb2c14be)


**Interrupt capture with Ctrl-C**

![interrupt_capture](https://github.com/user-attachments/assets/81502c65-51b0-499e-8a57-bef6842c7b9c)


---

## Contributing

Feel free to open issues or submit pull requests. Possible enhancements:

- Add per-second throughput statistics and CSV export
- Implement simple IDS alerts (e.g., port scan, SYN flood detection)
- Multi-threaded capture → parse → log pipeline
- Web-based or GUI visualization of live traffic

---

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.


# Telnet Client

### Dependency installation

* **1. Make sure to install:**

```bash
xcode-select --install
brew install cmake
```

* **2. Project assembly:**

```bash
cd telnet_client
mkdir build
cd build
cmake ..
make
```

* **3. Project assembly:**

```bash
./telnet_client example.com 80
```

* **4. Query entry:**

```bash
> GET / HTTP/1.1
> Host: example.com
> Connection: close
> END
```

* **5.Rebuild the project:**

```bash
cd build
cmake --build . --clean-first
```
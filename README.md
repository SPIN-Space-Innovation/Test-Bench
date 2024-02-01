# Embedded Template

## Building the Project
Before you can run the application, you'll need to build the project. Follow the steps below:

### Prerequisites
Make sure you have the following prerequisites installed on your machine:
- [CMake](https://cmake.org/download/) (version 3.19+)
- [GNU Arm Embedded Toolchain](https://developer.arm.com/downloads/-/gnu-rm) (version 10+)
- [tio](https://github.com/tio/tio) (optional for stdio over usb)

### Clone the Repository
```bash
git clone https://github.com/SPIN-Space-Innovation/Embedded-Template.git
cd Embedded-Template
```

### Generate the project buildsystem
Use the following command to generate the project's buildsystem:
```bash
cmake -B build -DCMAKE_BUILD_TYPE=${Build type} -G ${Buildsystem}
```

### Build the Project
Use the following command to build the project:
```bash
cmake --build build -j ${No of build jobs}
```

## License
Embedded Template is licensed under the [MIT license](LICENSE).

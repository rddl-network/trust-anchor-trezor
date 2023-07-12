# trust-anchor-trezor

This repository contains the necessary files to deploy the stack for the trusted-anker platform using the CPU ESP-C3-32S. The boards folder contains custom firmware for the rddl board and the lib folder contains the required libraries for communication with the Trusted-anker, and cryptography.

## Requirements

To use this repository, you have two options:

1. Use the PlatformIO CLI Core: You can install it by following the instructions on the official [PlatformIO website](https://docs.platformio.org/en/latest/core/installation.html).
2. Use Visual Studio Code: If you prefer using an IDE, you can install Visual Studio Code and the PlatformIO extension. Follow the instructions on the official [PlatformIO website](https://docs.platformio.org/en/latest/integration/ide/vscode.html) to set up the extension.

This Repository contains submodules so you need to run:
```git submodule update --init --recursive```

## Deployment

To deploy the application, run the following command:

```pio run --target upload```

If you are using Visual Studio Code, you can deploy the application by clicking on the PlatformIO icon on the left-hand side, selecting your board, and clicking on the "Upload" button.

This will compile and upload the firmware to the ESP-C3-32S CPU. Please make sure that your board is connected to your system and that you have selected the correct port in the `platformio.ini` file.

## Monitoring

To monitor the application, run the following shell command:

```pio device monitor```

If you are using Visual Studio Code, you can monitor the application by clicking on the PlatformIO icon on the left-hand side, selecting your board, and clicking on the "Monitor" button.

This will open the serial monitor, where you can see the output of the application and interact with it.

## Libraries

The following libraries are used in this project and are included in the `lib` folder:

- OSC: used for communication with the Trusted-anker
- Trezor: used for cryptography
- seal-edge-apdu: used for communication with se050

## Contributing

If you have any suggestions or improvements, please feel free to contribute to this repository. You can open a pull request and we will review it as soon as possible.

## To DO

- There are architecture specific files in the Trezor library, a python script was used to remove them. Instead of this method, a more formal way should be followed. (DONE)

## Notes!

Libs are moved under src directory to use "build_src_filter" in platformio.ini file. 

## License

This repository is licensed under the MIT License. See the `LICENSE` file for more information.

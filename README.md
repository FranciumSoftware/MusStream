# MusStream
MusStream is a music streaming application built using the Qt framework. It allows users to stream their favorite music from various online sources, create playlists, and manage their music library.
## Features
- Add sources: Users can add their preferred music streaming sources to the application.
- Manage sources: Users can manage their added sources, including editing and removing them.
- Search for titles: Users can search for specific music titles across their added sources.
- Play music: Users can play their favorite music directly from the application.

## Roadmap
See [Roadmap.md](Roadmap.md) for detailed information on upcoming features and release dates.

## Installation
### Windows
1. Download the installer from the latest release on GitHub.
2. Run the installer and follow the steps.
3. Launch MusStream and enjoy!
### Linux
You can't download the binaries yet, follow the build instructions in **Compiling** section.
### macOS
Same as Linux, follow the build instructions in **Compiling** section.

## Compiling
1. Clone the repository:
```bash
git clone https://github.com/FranciumSoftware/MusStream.git
```
> [!IMPORTANT]
> Make sure you have the Qt framework installed on your system. You can download it from the official Qt website: https://www.qt.io/download
### Windows
Open the project in Qt Creator and build it using the provided configuration.
##### OR
Use the command line to build the project:
```bash
cd MusStream
cmake -S . -B build
cmake --build build --config Release
```
### Linux and macOS
Open the terminal and navigate to the project directory, then run the following commands:
```bash
cd MusStream
cmake -S . -B build
cmake --build build --config Release
```
## Contributing
Contributions are welcome! If you want to contribute to MusStream, please follow these steps:
1. Fork the repository on GitHub.
2. Create a new branch for your feature or bug fix.
3. Make your changes and commit them with descriptive messages.
4. Push your changes to your forked repository.
5. Open a pull request to the main repository with a clear description of your changes.

## License
Under GPL-3.0 License. Go to [LICENSE](LICENSE) for more details.

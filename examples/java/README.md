# Scanbot SDK for Linux – Java Example

### Setup

Open `build.gradle` and replace the constant with the actual version number of the SDK you want to install.
   ```groovy
   def SCANBOTSDK_VERSION = "<SCANBOTSDK_VERSION>" // ex: 0.800.3
   ```  

## Usage
The example supports four commands: **scan**, **analyze**, **classify**, and **parse**.
```bash
./gradlew run --args='scan <subcommand> --file <path/to/file.jpg> [--license <KEY>]'
./gradlew run --args='scan <subcommand> --resource <path/to/resource.jpg> [--license <KEY>]'
./gradlew run --args='analyze <subcommand> --file <path/to/file.jpg> --save <out.jpg> [--license <KEY>]'
./gradlew run --args='analyze <subcommand> --resource <path/to/resource.jpg> --save <out.jpg> [--license <KEY>]'
./gradlew run --args='classify <subcommand> --file|--resource <path/to/file.jpg> [--license <KEY>]'
./gradlew run --args='parse <subcommand> --text "<input>" [--license <KEY>]'
```

## Example
```bash
./gradlew run --args='scan barcode --file images/example.jpg --license <KEY>'
./gradlew run --args='analyze analyse_multi_page --resource files/doc.pdf --license <KEY>'
./gradlew run --args='analyze crop_analyze --file images/doc.jpg --save out/crop.jpg --license <KEY>'
./gradlew run --args='parse mrz --text "P<UTOERIKSSON<<ANNA<MARIA<<<<<<" --license <KEY>'
```

## API References
👉 [Scanbot SDK Java API Documentation](https://scanbotsdk.github.io/documentation/document-scanner-sdk/linux/java-api/)

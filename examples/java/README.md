# Scanbot SDK for Linux – Java Example
This repository demonstrates how to use the [Scanbot SDK for Linux](https://scanbot.io) with Java.  

### Setup
1. Go to [GitHub Releases](https://github.com/doo/scanbot-sdk-example-linux/releases)  
2. Manually download the **Jar package** for your platform (ARM64 or x86_64).  
3. Place the SDK JAR `scanbotsdk-<SCANBOTSDK_VERSION>-linux-<SCANBOTSDK_ARCH>.jar` into the `libs/` folder.
4. Open `build.gradle` and set the constants:  
   ```groovy
   def SCANBOTSDK_VERSION = "<SCANBOTSDK_VERSION>" // ex: 0.800.3
   def SCANBOTSDK_ARCH    = "<SCANBOTSDK_ARCH>"   // x86_64 or aarch64
   ```  

## Usage
The example supports four modes: **scan**, **analyze**, **classify**, and **parse**.
```bash
# Scan commands
./gradlew run --args='scan <command> --file <path/to/file.jpg> [--license <KEY>]'
./gradlew run --args='scan <command> --resource <path/to/resource.jpg> [--license <KEY>]'

# Analyze commands
./gradlew run --args='analyze <command> --file <path/to/file.jpg> --save <out.jpg> [--license <KEY>]'
./gradlew run --args='analyze <command> --resource <path/to/resource.jpg> --save <out.jpg> [--license <KEY>]'

# Classify commands
./gradlew run --args='classify <command> --file|--resource <path/to/file.jpg> [--license <KEY>]'

# Parse commands
./gradlew run --args='parse <command> --text "<input>" [--license <KEY>]'
```

## Example
```bash
# Scan a barcode from a local file
./gradlew run --args='scan barcode --file images/example.jpg --license <KEY>'

# Analyze quality of a multipage PDF
./gradlew run --args='analyze analyse_multi_page --resource files/doc.pdf --license <KEY>'

# Crop & analyze a document image, saving the cropped result
./gradlew run --args='analyze crop_analyze --file images/doc.jpg --save out/crop.jpg --license <KEY>'

# Parse MRZ text input
./gradlew run --args='parse mrz --text "P<UTOERIKSSON<<ANNA<MARIA<<<<<<" --license <KEY>'
```
## API References
👉 [Scanbot SDK Java API Documentation](https://scanbotsdk.github.io/documentation/document-scanner-sdk/linux/java-api/)

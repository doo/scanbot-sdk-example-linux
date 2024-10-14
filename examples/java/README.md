# Scanbot SDK for Linux - Java Wrapper Example

The [Scanbot SDK for Linux](https://scanbot.io) is currently in closed beta. 
The SDK and a trial license are available on request via beta@scanbot.io.

## How to run the example

This example app can be run only on the Linux platform.

To run the example you need to

1. Specify values of `SCANBOTSDK_VERSION` and `SCANBOTSDK_API_TOKEN` in the `build.gradle` file with the values you received
   from us.
2. Specify your license key in the `io.scanbot.sdk.ScanbotSDKExample` class with the value you received
   from us.

`SCANBOTSDK_VERSION` and `SCANBOTSDK_API_TOKEN`, and a trial license key  available on request via beta@scanbot.io.

You can run the example on an image by its file path or its resource path. 

To run the example on an image by its file path, run the following command:

`./gradlew run --args='<command> --file "path/to/file.jpg" --save "path/to/save/image.jpg"'`

To run the example on an image by its resource path, run the following command:

`./gradlew run --args='<command> --resource "path/to/bundle/resource.jpg" --save "path/to/save/image.jpg"'`

The `<command>` can be one of the following:

* `analyzeDocument`: analyze document quality in the image.
* `detectDocument`: detect document in the image.
* `cropAndAnalyzeDocument`: detect document in the image and analyze document quality in the crop. Additionally, allows to save the detected document crop to the path specified with `--save` parameter.

In particular, the example comes with test images that can be used to run the example. To run the example on a test image run the following command:

`./gradlew run --args='cropAndAnalyzeDocument --resource "/img2.jpg" --save "/tmp/scanbotsdk/cropped.jpg"'`

During the script run all needed artifacts will be downloaded and the example will be executed.

Note: The Scanbot SDK is distributed as a jar that contains native shared library. Each time when you run the example,
the native shared library will be extracted to a temporary directory.
If you want to avoid this, you can extract the native shared library and locate them in a path that is specified in
the `java.library.path` system property. In order to prevent the extraction of the native shared library, you should set
system property `unpack_scanbotsdk_resources` to `false`.


## API References

You can find the Java API docs [here](https://scanbotsdk.github.io/documentation/document-scanner-sdk/linux/java-api/).

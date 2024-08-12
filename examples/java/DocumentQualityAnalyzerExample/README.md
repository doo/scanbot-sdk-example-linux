# Scanbot SDK Java Wrapper Example

## How to run the example

To run the example you need to

1. Specify values of SCANBOTSDK_VERSION and SCANBOTSDK_API_TOKEN in the `build.gradle` file with the values you received
   from us.
2. Specify your license key in the `io.scanbot.sdk.dqa.DocumentQualityAnalyzerExample` class with the value you received
   from us.

After that you can run the example with the following command:
`./gradlew run`

During the script run all needed artifacts will be downloaded and the example will be executed.

Note: The ScanbotSDK is distributed as a jar that contains native shared library. Each time when you run the example,
the native shared library will be extracted to a temporary directory.
If you want to avoid this, you can extract the native shared library and locate them in a path that is specified in
the `java.library.path` system property. In order to prevent the extraction of the native shared library, you should set
system property `unpack_scanbotsdk_resources` to `false`.

# API explanation

The Document Quality Analyzer is configurable with `io.scanbot.sdk.dqa.Config` object. The properties of the `Config`:

* `qualityThresholds`: quality thresholds used to separate different quality levels. A quality threshold represent a
  requirement computed on a symbol histogram. Specifically, such a requirement means that at least @symbolRatio of the
  symbols in the image must have a quality @symbolQuality or lower.
* `qualityIndices`: quality indices used to represent the quality levels. The size of the list should greater by one
  than the size of the quality thresholds list.
* `maxImageSize`: maximum size of the image that will be processed. If the image is larger than this value, it will be
  resized before processing. Default is 2000.
* `minEstimatedNumberOfSymbolsForDocument`: minimum estimated number of symbols in the document required the document to
  be considered as found.Default is 20.
* `minProcessedFraction`: minimum fraction of the image that must be processed. The value should be in the range (0, 1].
  If the fraction of the image is larger than one tile, multiple tiles are used. Default is 0.0;
* `maxProcessedFraction`: maximum fraction of the image that must be processed. The value should be in the range (0, 1]. Default is 0.5.
* `earlyStopIfNSymbolsFound`: number of symbols that must be found to stop the analysis early. Default is 100.
* `tileSize`: instead of processing the entire image, the image is divided into tiles and in order to make the algorithm
  faster only the most relevant tiles are processed. The size of the tiles is determined by this parameter. Default is 300.
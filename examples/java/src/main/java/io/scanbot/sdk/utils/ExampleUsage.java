package io.scanbot.sdk.utils;

public class ExampleUsage {
    public static void print() {
        System.out.println("Scanbot SDK Example");
        System.out.println();
        System.out.println("Usage:");
        System.out.println("  ./gradlew run --args='scan <command> --resource <path/to/resource.jpg>'");
        System.out.println("or");
        System.out.println("  ./gradlew run --args='scan <command> --file <path/to/file.jpg>'");
        System.out.println("or");
        System.out.println("  ./gradlew run --args='classify <command> --file|--resource <path/to/file.jpg>'");
        System.out.println();
        System.out.println("  ./gradlew run --args='analyze <command> --resource <path/to/resource.jpg> --save <path/to/save.jpg>'");
        System.out.println("or");
        System.out.println("  ./gradlew run --args='analyze <command> --file <path/to/file.jpg> --save <path/to/save.jpg>'");
        System.out.println();
        System.out.println("  ./gradlew run --args='parse <command> --text \"<input>\"'");
        System.out.println();
        System.out.println("Available scan commands:");
        System.out.println("  barcode | document | check | credit_card | document_extractor | medical_certificate | mrz | ocr | text_pattern | vin");
        System.out.println();
        System.out.println("Available analyze commands:");
        System.out.println("  analyse_multi_page | crop_analyze");
        System.out.println();
        System.out.println("Available classify commands:");
        System.out.println("  document");
        System.out.println();
        System.out.println("Available parse commands:");
        System.out.println("  mrz | barcode_doc");
        System.out.println();
        System.out.println("Note:");
        System.out.println("  The --save argument is optional and only used with analyze/crop_analyze.");
        System.out.println();
        System.out.println("Examples:");
        System.out.println("  ./gradlew run --args='scan barcode --file images/example.jpg'");
        System.out.println("  ./gradlew run --args='analyze analyse_multi_page --resource files/doc.pdf'");
        System.out.println("  ./gradlew run --args='analyze crop_analyze --file images/doc.jpg --save out/crop.jpg'");
        System.out.println("  ./gradlew run --args='parse mrz --text \"P<UTOERIKSSON<<ANNA<MARIA<<<<<<\"'");
        System.out.println();
    }
}

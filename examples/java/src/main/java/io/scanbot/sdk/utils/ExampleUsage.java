package io.scanbot.sdk.utils;

public class ExampleUsage {
    public static void print() {
        System.out.println("Scanbot SDK Example");
        System.out.println();
        System.out.println("Usage:");
        System.out.println("  ./gradlew run --args='<command> --resource <path/to/resource.jpg> --save <path/to/save.jpg>'");
        System.out.println("or");
        System.out.println("  ./gradlew run --args='<command> --file <path/to/file.jpg> --save <path/to/save.jpg>'");
        System.out.println();
        System.out.println("Available commands:");
        System.out.println("  detectDocument");
        System.out.println("  analyzeMultiPageDocument");
        System.out.println("  cropAndAnalyzeDocument");
        System.out.println();
        System.out.println("Note:");
        System.out.println("  The --save argument is optional and only used with cropAndAnalyzeDocument.");
        System.out.println();
    }
}

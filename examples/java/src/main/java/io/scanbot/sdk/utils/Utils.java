package io.scanbot.sdk.utils;

import java.io.IOException;
import java.io.InputStream;
import java.util.LinkedHashMap;
import java.util.Map;

import io.scanbot.sdk.genericdocument.Field;
import io.scanbot.sdk.genericdocument.GenericDocument;
import io.scanbot.sdk.genericdocument.GenericDocumentType;
import io.scanbot.sdk.image.BufferImageLoadOptions;
import io.scanbot.sdk.image.ImageRef;
import io.scanbot.sdk.image.PathImageLoadOptions;
import io.scanbot.sdk.io.RandomAccessSource;
import io.scanbot.sdk.multipageimageextractor.MultiPageImageExtractor;
import io.scanbot.sdk.multipageimageextractor.PageExtractionResult;

public class Utils {
    public static ImageRef createImageRef(String filePath, String resourcePath) throws IOException {
        if (filePath != null) {
            return ImageRef.fromPath(filePath, new PathImageLoadOptions());
        }

        try (InputStream stream = Utils.class.getResourceAsStream(resourcePath)) {
            return ImageRef.fromStream(stream, new BufferImageLoadOptions());
        }
    }

    public static PageExtractionResult extractImages(String filePath, String resourcePath) throws Exception {
        try (
            RandomAccessSource source = (filePath != null)
                ? new RandomAccessSource(filePath)
                : new RandomAccessSource(Utils.class.getResourceAsStream(resourcePath));
            MultiPageImageExtractor extractor = new MultiPageImageExtractor()
        ) {
            return extractor.run(source);
        }
    }

    public static void printGenericDocument(GenericDocument doc) {
        if (doc == null) {
            System.out.println("Document: None");
            return;
        }

        GenericDocumentType type = doc.getType();
        String typeName = (type != null) ? type.getName() : "—";
        String typeFullName = (type != null) ? type.getFullName() : "—";
        System.out.printf("Type: %s (%s)%n", typeName, typeFullName);

        System.out.println("Fields:");
        for (Field field : doc.getFields()) {
            String fieldName = (field.getType() != null) ? field.getType().getName() : "—";
            String valueText = (field.getValue() != null) ? field.getValue().getText() : null;
            System.out.printf("%s: %s%n", fieldName, valueText);
        }
        System.out.println("Children:");
        for (GenericDocument child : doc.getChildren()) {
            printGenericDocument(child);
        }
    }

    public static Map<String, String> parseFlags(String[] a) {
        Map<String, String> m = new LinkedHashMap<>();
        for (int i = 0; i < a.length; ) {
            String k = a[i];
            if (!k.startsWith("--")) { System.err.println("Unexpected token: " + k); break; }
            String v = (i + 1 < a.length && !a[i + 1].startsWith("--")) ? a[++i] : "true";
            m.put(k, v);
            i++;
        }
        return m;
    }
}

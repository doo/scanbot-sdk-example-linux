import * as ScanbotSDK from "scanbotsdk";

export async function extractImages(
  filePath: string
): Promise<ScanbotSDK.PageExtractionResult> {
    await using source = await ScanbotSDK.RandomAccessSource.fromPath(filePath);
    await using extractor = await ScanbotSDK.MultiPageImageExtractor.create();

    return await extractor.run(source);
}

export function printGenericDocument(
  doc?: ScanbotSDK.GenericDocument | null
): void {
  if (!doc) {
    console.log("Document: None");
    return;
  }

  const typeName = doc.type.name;
  const typeFullName = doc.type.fullName;
  console.log(`Type: ${typeName} (${typeFullName})`);

  if (doc.fields && doc.fields.length > 0) {
    console.log("Fields:");
    for (const field of doc.fields) {
      const fieldName = field.type ? field.type.name : "—";
      const valueText = field.value ? field.value.text : null;
      console.log(`${fieldName}: ${valueText}`);
    }
  }

  if (doc.children && doc.children.length > 0) {
    console.log("Children:");
    for (const child of doc.children) {
      printGenericDocument(child);
    }
  }
}

export function parseFlags(argv: string[]): Record<string, string | true> {
  const res: Record<string, string | true> = {};
  for (let i = 0; i < argv.length; i++) {
    const a = argv[i];
    if (a?.startsWith("--")) {
      const next = argv[i + 1];
      if (!next || next.startsWith("--")) {
        res[a] = true;
      } else {
        res[a] = next;
        i++;
      }
    }
  }
  return res;
}

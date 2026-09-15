export function printUsage(): void {
  console.log(`
Usage:
  npx ts-node src/index.ts <category> <subcommand> [--file <path>] [--save <path>] [--text <value>] [--license <key>]
  npx ts-node src/index.ts enhance <subcommand> --file <path> [--save <path>] [--mask <path>] [--license <key>]

Categories & subcommands:
  scan <barcode | document  | check | credit_card | document_data_extractor  | medical_certificate | mrz | ocr | text_pattern  | vin>
  analyze <analyze_multi_page | crop_analyze>
  enhance <straighten_document | cleanup_document>
  parse <mrz  | barcode_doc>

Flags:
  --file <path>       Path to an input image/file on disk
  --save <path>       Path to save outputs
  --mask <path>       Path to cleanup mask image (required for enhance/cleanup_document)
  --text <value>      Text input for parsers
  --license <key>     Scanbot SDK license key (optional).
                      If not provided, the placeholder variable "<SCANBOTSDK-LICENSE>" is applied.

Examples:
  npx ts-node src/index.ts scan barcode --file images/example.jpg --license <KEY>
  npx ts-node src/index.ts analyze analyze_multi_page --file files/doc.pdf --license <KEY>
  npx ts-node src/index.ts analyze crop_analyze --file images/doc.jpg --save out/crop.jpg --license <KEY>
  npx ts-node src/index.ts enhance straighten_document --file images/doc.jpg --license <KEY>
  npx ts-node src/index.ts enhance cleanup_document --file images/credit_card.png --mask images/credit_card_mask.png --save out/cleaned.jpg --license <KEY>
  npx ts-node src/index.ts parse mrz --text "P<UTOERIKSSON<<ANNA<MARIA<<<<<<" --license <KEY>
`);
}

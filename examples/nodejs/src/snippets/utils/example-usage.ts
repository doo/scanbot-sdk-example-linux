export function printUsage(): void {
  console.log(`
Usage:
  node dist/index.js <category> <subcommand> [--file <path>] [--save <path>] [--text <value>]

Categories & subcommands:
  scan <barcode|document|check|credit_card|document_extractor|medical_certificate|mrz|ocr|text_pattern|vin>
  analyse <analyse_multi_page|crop_analyze>
  classify <document>
  parse <mrz|barcode_doc>

Flags:
  --file <path>       Path to an input image/file on disk
  --save <path>       Path to save outputs
  --text <value>      Text input for parsers
`);
}

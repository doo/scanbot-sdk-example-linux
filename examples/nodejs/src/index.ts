import * as ScanbotSDK from "scanbotsdk";

import { printUsage } from "./snippets/utils/example-usage";
import { parseFlags } from "./snippets/utils/utils";
import { DetectBarcodesSnippet } from "./snippets/barcode/detect-barcodes";
import { DetectDocumentSnippet } from "./snippets/document/detect-document";
import { CheckScannerSnippet } from "./snippets/datacapture/check-scanner";
import { CreditCardScannerSnippet } from "./snippets/datacapture/credit-card-scanner";
import { DocumentDataExtractorSnippet } from "./snippets/datacapture/document-data-extractor";
import { MedicalCertificateScannerSnippet } from "./snippets/datacapture/medical-certificate-scanner";
import { MrzScannerSnippet } from "./snippets/datacapture/mrz-scanner";
import { TextPatternScannerSnippet } from "./snippets/datacapture/text-pattern-scanner";
import { OcrSnippet } from "./snippets/datacapture/ocr";
import { VinScannerSnippet } from "./snippets/datacapture/vin-scanner";
import { DocumentClassifierSnippet } from "./snippets/document/document-classifier";
import { MrzParserSnippet } from "./snippets/datacapture/mrz-parser";
import { ParseBarcodeDocumentSnippet } from "./snippets/barcode/parse-barcode-document";
import { AnalyzeMultiPageSnippet } from "./snippets/document/analyze-multipage";
import { CropAndAnalyzeSnippet } from "./snippets/document/crop-analyze";

async function awaitPromise(promise: Promise<void>, maxAwaitTimeMs: number = 60 * 1000): Promise<void> {
  const timer = setTimeout(() => {
    throw new Error("Operation timed out");
  }, maxAwaitTimeMs);
  await promise;
  clearTimeout(timer);
}

async function main(): Promise<void> {
  if (process.argv.length < 2) {
    printUsage();
    return;
  }

  const args = process.argv.slice(2);

  const category = args[0]?.toLowerCase() ?? "";
  const subcommand = args[1]?.toLowerCase() ?? "";
  const flags = parseFlags(args.slice(2));

  const file = flags["--file"] as string | undefined;
  const save = flags["--save"] as string | undefined;
  const text = flags["--text"] as string | undefined;
  const licenseFlag = flags["--license"] as string | undefined;

  const licenseKey: string = licenseFlag ?? "<SCANBOTSDK-LICENSE>";
  const writablePath = ".";

  await ScanbotSDK.initialize({
    licenseKey,
    writablePath,
  });

  const licenseInfo = await ScanbotSDK.getLicenseInfo();
  console.log("License Status:", licenseInfo.status);

  const isFloatingLicense = !!licenseInfo.devices;
  if (isFloatingLicense) {
    console.log(
      `Using floating license with ${licenseInfo.devices} devices. Do not forget to call deregisterDevice ` +
        `when you no longer need the license.`
    );
  }

  // `ScanbotSDK.autorelease` automatically disposes all unmanaged resources
  // (e.g. ImageRefs, scanners, results) created inside this block when it ends.
  // This prevents memory leaks and avoids having to call `.release()` manually.
  //
  // Alternatively, you can use `await using` on individual
  // objects for more fine-grained control.
  //
  // Both patterns serve the same purpose: ensuring resources are freed in time.
  await ScanbotSDK.autorelease(async () => {
    switch (category) {
      case "scan": {
        if (!file) { printUsage(); return; }
        const image = await ScanbotSDK.ImageRef.fromPath(file);

        switch (subcommand) {
          case "barcode":             await DetectBarcodesSnippet.run(image); break;
          case "document":            await DetectDocumentSnippet.run(image); break;
          case "check":               await CheckScannerSnippet.run(image); break;
          case "credit_card":         await CreditCardScannerSnippet.run(image); break;
          case "document_extractor":  await DocumentDataExtractorSnippet.run(image); break;
          case "medical_certificate": await MedicalCertificateScannerSnippet.run(image); break;
          case "mrz":                 await MrzScannerSnippet.run(image); break;
          case "ocr":                 await OcrSnippet.run(image); break;
          case "text_pattern":        await TextPatternScannerSnippet.run(image); break;
          case "vin":                 await VinScannerSnippet.run(image); break;
          default: printUsage();
        }
        break;
      }

      case "analyze": {
        if (!file) { printUsage(); return; }

        switch (subcommand) {
          case "analyze_multi_page":  await AnalyzeMultiPageSnippet.run(file); break;
          case "crop_analyze":        await CropAndAnalyzeSnippet.run(file, save); break;
          default: printUsage();
        }
        break;
      }

      case "classify": {
        if (!file) { printUsage(); return; }
        const image = await ScanbotSDK.ImageRef.fromPath(file);

        switch (subcommand) {
          case "document":            await DocumentClassifierSnippet.run(image); break;
          default: printUsage();
        }
        break;
      }

      case "parse": {
        if (!text || text.trim().length === 0) { printUsage(); return; }

        switch (subcommand) {
          case "mrz":                 await MrzParserSnippet.run(text); break;
          case "barcode_doc":         await ParseBarcodeDocumentSnippet.run(text); break;
          default: printUsage();
        }
        break;
      }

      default: printUsage();
    }
  });
}

awaitPromise(main());
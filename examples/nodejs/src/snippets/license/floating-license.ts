import * as ScanbotSDK from "scanbotsdk";

export class FloatingLicenseSnippet {  
  public static async initWithFloatingLicense(): Promise<void> {
    const SCANBOTSDK_LICENSE_CHECK_TIMEOUT_MS = 15_000;
    const licenseKey = "SCANBOTSDK-LICENSE";
    const writablePath = ".";

    await ScanbotSDK.initialize({
      licenseKey,
      writablePath,
    });

    // Create device session with timeout (15 seconds). 
    // The session will be automatically disposed of when the scope ends.
    await using deviceSession = new ScanbotSDK.DeviceSession(SCANBOTSDK_LICENSE_CHECK_TIMEOUT_MS);

    let afterInfo = await ScanbotSDK.getLicenseInfo();
    console.log(
      `status=${afterInfo.status}, online_check_in_progress=${afterInfo.onlineLicenseCheckInProgress}`
    );
  }
}

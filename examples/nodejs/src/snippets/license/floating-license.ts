import * as ScanbotSDK from "scanbotsdk";

export class FloatingLicenseSnippet {
  public static async initWithFloatingLicense(): Promise<void> {
    const licenseKey = "SCANBOTSDK-LICENSE";
    const writablePath = ".";

    await ScanbotSDK.initialize({
      licenseKey,
      writablePath,
    });

    // Create device session with timeout (15 seconds)
    const deviceSession = new ScanbotSDK.DeviceSession(15_000);

    try {
      // BEFORE waiting for online check
      let beforeInfo = await ScanbotSDK.getLicenseInfo();
      console.log(
        `[before wait] status=${beforeInfo.status}, online_check_in_progress=${beforeInfo.onlineLicenseCheckInProgress}`
      );

      // Wait for the online license check (max 15s)
      //   await ScanbotSDK.waitForOnlineLicenseCheckCompletion(15_000);

      // AFTER waiting for online check
      let afterInfo = await ScanbotSDK.getLicenseInfo();
      console.log(
        `[after wait]  status=${afterInfo.status}, online_check_in_progress=${afterInfo.onlineLicenseCheckInProgress}`
      );
    } finally {
      // Always close session
      //   deviceSession.close();
    }

    // Deregister device
    await ScanbotSDK.deregisterDevice();
    // await ScanbotSDK.waitForDeviceDeregistrationCompletion(15_000);
  }
}

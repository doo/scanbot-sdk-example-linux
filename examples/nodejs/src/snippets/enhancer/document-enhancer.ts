import * as ScanbotSDK from "scanbotsdk";
import { AspectRatio } from "scanbotsdk";

export class DocumentEnhancerSnippet {
  public static async run(image: ScanbotSDK.ImageRef): Promise<void> {
    var params = new ScanbotSDK.DocumentStraighteningParameters();
    params.straighteningMode = "STRAIGHTEN";
    params.aspectRatios =[
      new AspectRatio({ width: 5.0, height: 7.0 }),
      new AspectRatio({ width: 1.0, height: 1.0 }),
      new AspectRatio({ width: 16.0, height: 9.0 }),
      new AspectRatio({ width: 3.0, height: 4.0 })
    ];

    // `await using` ensures both enhancer and result are properly disposed
    // when the scope ends, as they hold unmanaged resources.
    await using enhancer = await ScanbotSDK.DocumentEnhancer.create();
    await using result = await enhancer.straighten(image, params);

    // The straightened image can be accessed via result.straightenedImage and saved or further processed as needed.
  }
}

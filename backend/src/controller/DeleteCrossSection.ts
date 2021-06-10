import { Request, Response } from "express";
import { getManager } from "typeorm";
import Result from "../dto/Result";
import { CrossSection } from "../entity/CrossSection";

export async function deleteCrossSection(request: Request, response: Response) {

   let result = new Result<null>();

   await getManager().getRepository(CrossSection)
      .delete(request.params.id)
      .catch((e) => {
         result.success = false;
         result.message = "Cannot delete CrossSection";
      });

   response.send(result);
}

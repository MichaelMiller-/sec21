import { Request, Response } from "express";
import { getManager } from "typeorm";
import Result from "../dto/Result";
import { CurveMember } from "../entity/CurveMember";

export async function deleteCurveMember(request: Request, response: Response) {

   let result = new Result<null>();

   await getManager().getRepository(CurveMember)
      .delete(request.params.id)
      .catch((e) => {
         result.success = false;
         result.message = "Cannot delete Curve";
      });

   response.send(result);
}

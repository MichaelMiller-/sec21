import { Request, Response } from "express";
import { getManager } from "typeorm";
import Result from "../dto/Result";
import { Material } from "../entity/Material";

export async function deleteMaterial(request: Request, response: Response) {

   let result = new Result<null>();

   await getManager().getRepository(Material)
      .delete(request.params.id)
      .catch((e) => {
         result.success = false;
         result.message = "Cannot delete Member";
      });

   response.send(result);
}

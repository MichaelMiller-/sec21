import { Request, Response } from "express";
import { getManager } from "typeorm";
import { StructuralPoint } from "../entity/StructuralPoint"
import Result from "../dto/Result";

export async function deleteStructuralPoint(request: Request, response: Response) {

   let result = new Result<null>();

   await getManager().getRepository(StructuralPoint)
      .delete(request.params.id)
      .catch((e) => {
         result.success = false;
         if (e.code === '23503') {
            result.message = "Cannot delete StructuralPoint because it is still referenced";
         }
         else
            result.message = "Cannot delete StructuralPoint";
      });

   response.send(result);
}

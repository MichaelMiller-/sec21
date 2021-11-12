import { Request, Response } from "express";
import { getManager } from "typeorm";
import Result from "../dto/Result";
import { PointSupport } from "../entity/PointSupport";

export async function deletePointSupport(request: Request, response: Response) {

   let result = new Result<null>();

   await getManager().getRepository(PointSupport)
      .delete(request.params.id)
      .catch(() => {
         result.success = false;
         result.message = "Cannot delete Point-Support";
      });

   response.send(result);
}

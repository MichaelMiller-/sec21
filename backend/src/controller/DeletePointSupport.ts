import { Request, Response } from "express";
import { getManager } from "typeorm";
import Result from "../dto/Result";
import { PointSupport } from "../entity/PointSupport";

export async function deletePointSupport(request: Request, response: Response) {

   let result = new Result<null>();

   await getManager().getRepository(PointSupport)
      .delete(request.params.id)
       .catch((ex) => {
           const msg = ex
           result.success = false
           result.message = msg.detail
       });

   response.send(result);
}

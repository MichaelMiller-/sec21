import { Request, Response } from "express";
import { getManager } from "typeorm";
import Result from "../dto/Result";
import { Project } from "../entity/Project";

export async function deleteProject(request: Request, response: Response) {

   let result = new Result<null>();

   await getManager().getRepository(Project)
      .delete(request.params.id)
      .catch((ex) => {
         const msg = ex
         result.success = false
         result.message = msg.detail
      });

   response.send(result);
}

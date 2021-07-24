import { Request, Response } from "express";
import { getManager } from "typeorm";
import Result from "../dto/Result";
import { LoadCase } from "../entity/LoadCase";
import { Project } from "../entity/Project";

export async function addLoadCase(request: Request, response: Response) {

   let result = new Result<null>();

   const prj = await getManager().getRepository(Project).findOne(request.params.id);

   if (getManager().getRepository(Project).hasId(prj) === false) {
      result.success = false;
      result.message = "Project not found"
      response.send(result);
      return;
   }

   let obj = new LoadCase();
   obj.name = request.body.name;
   obj.description = request.body.description;
   obj.project = prj;

   if (obj.name.length === 0) {
      result.success = false;
      result.message = "'name' cannot be empty"
      response.send(result);
      return;
   }

   getManager().getRepository(LoadCase)
      .save(obj)
      .then(() => {
         result.success = true;
         result.message = "";
      })
      .catch(ex => {
         result.success = false;
         result.message = ex;
      });

   response.send(result);
}

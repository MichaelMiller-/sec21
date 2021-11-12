import { Request, Response } from "express";
import { getManager } from "typeorm";
import Result from "../dto/Result";
import { Material } from "../entity/Material";
import { Project } from "../entity/Project";

export async function addMaterial(request: Request, response: Response) {

   let result = new Result<null>();

   const prj = await getManager().getRepository(Project).findOne(request.params.id);

   if (getManager().getRepository(Project).hasId(prj) === false) {
      result.success = false;
      result.message = "Project not found"
      response.send(result);
      return;
   }

   let obj = new Material();
   obj.name = request.body.name;
   obj.E_modulus = request.body.E_modulus;
   obj.project = prj;

   if (obj.name.length === 0) {
      result.success = false;
      result.message = "'name' cannot be empty"
      response.send(result);
      return;
   }

   getManager().getRepository(Material)
      .save(obj)
      .then(() => {
         result.success = true;
         result.message = "";
      })
      .catch(ex => {
         console.log(ex)
         result.success = false;
         result.message = "TODO: exception";
      });

   response.send(result);
}

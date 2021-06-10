import { Request, Response } from "express";
import { getManager } from "typeorm";
import Result from "../dto/Result";
import { CrossSection } from "../entity/CrossSection";
import { Project } from "../entity/Project";

export async function addCrossSection(request: Request, response: Response) {

   let result = new Result<null>();

   const prj = await getManager().getRepository(Project).findOne(request.params.id);

   if (getManager().getRepository(Project).hasId(prj) === false) {
      result.success = false;
      result.message = "Project not found"
      response.send(result);
      return;
   }

   //! \todo 
   // const mat = await getManager().getRepository(Material).findOne();

   // if (getManager().getRepository(Material).hasId(mat) === false) {
   //    result.success = false;
   //    result.message = "Material not found"
   //    response.send(result);
   //    return;
   // }

   let obj = new CrossSection();
   obj.name = request.body.name;
   obj.area = request.body.area;
   obj.project = prj;

   if (obj.name.length === 0) {
      result.success = false;
      result.message = "'name' cannot be empty"
      response.send(result);
      return;
   }

   getManager().getRepository(CrossSection)
      .save(obj)
      .then(() => {
         result.success = true;
         result.message = "";
      })
      .catch(() => {
         result.success = false;
         result.message = "TODO: exception";
      });

   response.send(result);
}

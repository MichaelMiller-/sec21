import { Request, Response } from "express";
import { getManager } from "typeorm";
import { CurveMember } from "../entity/CurveMember";
import { Project } from "../entity/Project";
import { StructuralPoint } from "../entity/StructuralPoint";
import Result from "../dto/Result";
import { CrossSection } from "../entity/CrossSection";

export async function addCurveMember(request: Request, response: Response) {

   let result = new Result<null>();

   const ref = await getManager().getRepository(Project).findOne(request.params.id);

   if (getManager().getRepository(Project).hasId(ref) === false) {
      result.success = false;
      result.message = "Project not found"
      response.send(result);
      return;
   }

   const fromPt = await getManager().getRepository(StructuralPoint).findOne(request.params.from);

   if (getManager().getRepository(StructuralPoint).hasId(fromPt) === false) {
      result.success = false;
      result.message = "Structural Point not found"
      response.send(result);
      return;
   }

   const toPt = await getManager().getRepository(StructuralPoint).findOne(request.params.to);

   if (getManager().getRepository(StructuralPoint).hasId(toPt) === false) {
      result.success = false;
      result.message = "Structural Point not found"
      response.send(result);
      return;
   }

   const cs = await getManager().getRepository(CrossSection).findOne(request.params.cs);

   if (getManager().getRepository(CrossSection).hasId(cs) === false) {
      result.success = false;
      result.message = "Cross Section not found"
      response.send(result);
      return;
   }

   let obj = new CurveMember();
   obj.name = request.body.name;
   obj.beginNode = fromPt;
   obj.endNode = toPt;
   obj.project = ref;
   obj.crossSection = cs;

   getManager().getRepository(CurveMember)
      .save(obj)
      .then(() => {
         result.success = true;
         result.message = "";
      })
      .catch(ex => {
         result.success = false;
         result.message = ex.detail;
      });

   response.send(result);
}

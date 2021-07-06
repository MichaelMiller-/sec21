import { Request, Response } from "express";
import { getManager } from "typeorm";
import { CurveMember } from "../entity/CurveMember";
import { Project } from "../entity/Project";
import { StructuralPoint } from "../entity/StructuralPoint";

export async function addCurveMember(request: Request, response: Response) {

   const ref = await getManager().getRepository(Project).findOne(request.params.id);

   if (getManager().getRepository(Project).hasId(ref) === false) {
      response.send({ "sucess": false, "message": "project not found" });
      return;
   }

   const fromPt = await getManager().getRepository(StructuralPoint).findOne(request.params.from);

   if (getManager().getRepository(StructuralPoint).hasId(fromPt) === false) {
      response.send({ "sucess": false, "message": "structural point is not found" });
      return;
   }

   const toPt = await getManager().getRepository(StructuralPoint).findOne(request.params.to);

   if (getManager().getRepository(StructuralPoint).hasId(toPt) === false) {
      response.send({ "sucess": false, "message": "structural point is not found" });
      return;
   }


   let obj = new CurveMember();
   obj.name = request.body.name;
   obj.beginNode = fromPt;
   obj.endNode = toPt;
   obj.project = ref;

   if (obj.name.length === 0) {
      response.send({ "sucess": false, "message": "name cannot be empty" });
      return;
   }

   getManager().getRepository(CurveMember)
      .save(obj)
      .then(() => { response.send({ "sucess": true, "message": "" }); })
      .catch(ex => { response.send({ "sucess": false, "message": ex }); });
}

import { Request, Response } from "express";
import { getManager } from "typeorm";
import { LoadCase } from "../entity/LoadCase";
import { PointAction } from "../entity/PointAction";
import { StructuralPoint } from "../entity/StructuralPoint";

export async function addPointAction(request: Request, response: Response) {

   const ref = await getManager().getRepository(LoadCase).findOne(request.params.loadcase);

   if (getManager().getRepository(LoadCase).hasId(ref) === false) {
      response.send({ "sucess": false, "message": "load case not found" });
      return;
   }

   const pt = await getManager().getRepository(StructuralPoint).findOne(request.params.pt);

   if (getManager().getRepository(StructuralPoint).hasId(pt) === false) {
      response.send({ "sucess": false, "message": "structural point is not found" });
      return;
   }

   let obj = new PointAction();
   obj.name = request.body.name;
   obj.value_x = request.body.value_x;
   obj.value_y = request.body.value_y;
   obj.value_z = request.body.value_z;
   obj.loadcase = ref;
   obj.referencePoint = pt;

   if (obj.name.length === 0) {
      response.send({ "sucess": false, "message": "name cannot be empty" });
      return;
   }

   getManager().getRepository(PointAction)
      .save(obj)
      .then(() => { response.send({ "sucess": true, "message": "" }); })
      .catch(ex => { response.send({ "sucess": false, "message": ex }); });
}

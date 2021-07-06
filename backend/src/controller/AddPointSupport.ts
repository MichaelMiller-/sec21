import { Request, Response } from "express";
import { getManager } from "typeorm";
import { PointSupport } from "../entity/PointSupport";
import { StructuralPoint } from "../entity/StructuralPoint";

export async function addPointSupport(request: Request, response: Response) {

   const pt = await getManager().getRepository(StructuralPoint).findOne(request.params.pt);

   if (getManager().getRepository(StructuralPoint).hasId(pt) === false) {
      response.send({ "sucess": false, "message": "structural point is not found" });
      return;
   }

   let obj = new PointSupport();
   obj.referencePoint = pt;
   obj.ux = request.body.ux;
   obj.uy = request.body.uy;
   obj.uz = request.body.uz;
   obj.fix = request.body.fix;
   obj.fiy = request.body.fiy;
   obj.fiz = request.body.fiz;
   obj.stiffnessX = 0
   obj.stiffnessY = 0
   obj.stiffnessZ = 0
   obj.stiffnessFix = 0
   obj.stiffnessFiy = 0
   obj.stiffnessFiz = 0

   getManager().getRepository(PointSupport)
      .save(obj)
      .then(() => { response.send({ "sucess": true, "message": "" }); })
      .catch(ex => { response.send({ "sucess": false, "message": ex }); });
}

import { Request, Response } from "express";
import { getManager } from "typeorm";
import { PointSupport } from "../entity/PointSupport";
import { StructuralPoint } from "../entity/StructuralPoint";
import Result from "../dto/Result";
import logger from "../logging/Logger";

export async function addPointSupport(request: Request, response: Response) {

   let result = new Result<null>();

   const pt = await getManager().getRepository(StructuralPoint).findOne(request.params.pt);

   if (getManager().getRepository(StructuralPoint).hasId(pt) === false) {
      result.success = false;
      result.message = "Structural Point not found"
      response.send(result);
      return;
   }

   let obj = new PointSupport();
   obj.name = request.body.name;
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
      .then(() => {
         result.success = true;
         result.message = "";
      })
      .catch(ex => {
         logger.error(ex);
         result.success = false;
         result.message = ex;
      });

   response.send(result);
}

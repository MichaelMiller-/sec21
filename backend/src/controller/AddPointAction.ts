import { Request, Response } from "express";
import { getManager } from "typeorm";
import { LoadCase } from "../entity/LoadCase";
import { PointAction } from "../entity/PointAction";
import { StructuralPoint } from "../entity/StructuralPoint";
import Result from "../dto/Result";
import logger from "../logging/Logger";

export async function addPointAction(request: Request, response: Response) {

   let result = new Result<null>();

   const ref = await getManager().getRepository(LoadCase).findOne(request.params.loadcase);

   if (getManager().getRepository(LoadCase).hasId(ref) === false) {
      result.success = false;
      result.message = "Loadcase not found"
      response.send(result);
      return;
   }

   const pt = await getManager().getRepository(StructuralPoint).findOne(request.params.pt);

   if (getManager().getRepository(StructuralPoint).hasId(pt) === false) {
      result.success = false;
      result.message = "Structural Point not found"
      response.send(result);
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
      result.success = false;
      result.message = "'name' cannot be empty"
      response.send(result);
      return;
   }

   getManager().getRepository(PointAction)
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

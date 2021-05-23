import { Request, Response } from "express";
import { getManager } from "typeorm";
import { Material } from "../entity/Material";
import { Project } from "../entity/Project";

export async function addMaterial(request: Request, response: Response) {

   const prj = await getManager().getRepository(Project).findOne(request.params.id);

   if (getManager().getRepository(Project).hasId(prj) === false) {
      response.send({ "sucess": false, "message": "project not found" });
      return;
   }

   let obj = new Material();
   obj.name = request.body.name;
   obj.E_modules = request.body.E_modules;

   if (obj.name.length === 0) {
      response.send({ "sucess": false, "message": "name cannot be empty" });
      return;
   }

   getManager().getRepository(Material)
      .save(obj)
      .then(obj => { response.send({ "sucess": true, "message": "" }); })
      .catch();
}

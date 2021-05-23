import { Request, Response } from "express";
import { getManager } from "typeorm";
import { CrossSection } from "../entity/CrossSection";
import { Project } from "../entity/Project";

export async function addCrossSection(request: Request, response: Response) {

   const prj = await getManager().getRepository(Project).findOne(request.params.id);

   if (getManager().getRepository(Project).hasId(prj) === false) {
      response.send({ "sucess": false, "message": "project not found" });
      return;
   }

   let obj = new CrossSection();
   obj.name = request.body.name;
   obj.area = request.body.area;

   if (obj.name.length === 0) {
      response.send({ "sucess": false, "message": "name cannot be empty" });
      return;
   }

   getManager().getRepository(CrossSection)
      .save(obj)
      .then(obj => { response.send({ "sucess": true, "message": "" }); })
      .catch();
}

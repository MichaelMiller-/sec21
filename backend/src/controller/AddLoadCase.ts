import { Request, Response } from "express";
import { getManager } from "typeorm";
import { LoadCase } from "../entity/LoadCase";
import { Project } from "../entity/Project";

export async function addLoadCase(request: Request, response: Response) {

   const prj = await getManager().getRepository(Project).findOne(request.params.id);

   if (getManager().getRepository(Project).hasId(prj) === false) {
      response.send({ "sucess": false, "message": "project not found" });
      return;
   }

   let obj = new LoadCase();
   obj.name = request.body.name;
   obj.description = request.body.description;
   obj.project = prj;

   if (obj.name.length === 0) {
      response.send({ "sucess": false, "message": "name cannot be empty" });
      return;
   }

   getManager().getRepository(LoadCase)
      .save(obj)
      .then(obj => { response.send({ "sucess": true, "message": "" }); })
      .catch();
}

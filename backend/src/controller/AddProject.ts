import { Request, Response } from "express";
import { getManager } from "typeorm";
import { Project } from "../entity/Project";

export async function addProject(request: Request, response: Response) {

    console.log("data: ", request.body);

    let prj = new Project();
    prj.name = request.body.name;
    prj.title = request.body.title;

    if (prj.name.length === 0) {
        response.send({ "sucess": false, "message": "name cannot be empty" });
        return;
    }

    getManager().getRepository(Project)
        .save(prj)
        .then(obj => { response.send({ "sucess": true, "message": "" }); })
        .catch();
}

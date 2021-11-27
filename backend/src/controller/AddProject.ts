import { Request, Response } from "express";
import { getManager } from "typeorm";
import { Project } from "../entity/Project";
import Result from "../dto/Result";

export async function addProject(request: Request, response: Response) {

    let result = new Result<null>();

    let obj = new Project();
    obj.name = request.body.name;
    obj.title = request.body.title;
    obj.createdAt = new Date(Date.now());
    obj.modifiedAt = new Date(Date.now());

    getManager().getRepository(Project)
        .save(obj)
        .then(() => {
            result.success = true;
            result.message = "";
        })
        .catch(ex => {
            result.success = false;
            result.message = ex;
        });

    response.send(result);
}

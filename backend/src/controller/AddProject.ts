import { Request, Response } from "express";
import { getManager } from "typeorm";
import { Project } from "../entity/Project";
import Result from "../dto/Result";

export async function addProject(request: Request, response: Response) {

    let result = new Result<null>();

    let prj = new Project();
    prj.name = request.body.name;
    prj.title = request.body.title;

    if (prj.name.length === 0) {
        result.success = false;
        result.message = "'name' cannot be empty"
        response.send(result);
        return;
    }

    if (prj.title.length === 0) {
        result.success = false;
        result.message = "'title' cannot be empty"
        response.send(result);
        return;
    }

    getManager().getRepository(Project)
        .save(prj)
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

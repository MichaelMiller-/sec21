import { Request, Response } from "express";
import { getManager } from "typeorm";
import { Project } from "../entity/Project";

export async function getProject(request: Request, response: Response) {
    const values = await getManager().getRepository(Project).findOne(request.params.id);
    response.send(values);
}

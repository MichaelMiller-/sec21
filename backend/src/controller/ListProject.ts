import { Request, Response } from "express";
import { getManager } from "typeorm";
import { Project } from "../entity/Project"

export async function listProjects(request: Request, response: Response) {
    const prjs = await getManager().getRepository(Project).find();
    response.send(prjs);
}
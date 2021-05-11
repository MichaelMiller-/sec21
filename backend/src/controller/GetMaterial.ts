import { Request, Response } from "express";
import { getManager } from "typeorm";
import { Material } from "../entity/Material";

export async function getMaterial(request: Request, response: Response) {
    const values = await getManager().getRepository(Material).findOne(request.params.id);
    response.send(values);
}

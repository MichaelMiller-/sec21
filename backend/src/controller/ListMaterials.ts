import { Request, Response } from "express";
import { getManager } from "typeorm";
import { Material } from "../entity/Material";

export async function listMaterials(request: Request, response: Response) {

    const values = await getManager()
        .getRepository(Material)
        .createQueryBuilder("material")
        .where("material.project = :id", { id: request.params.id })
        .orderBy("material.name", "ASC")
        .getMany();

    // const values = getManager().getRepository(StructuralPoint).find({ where: { project: 1 } });

    response.send(values);
}
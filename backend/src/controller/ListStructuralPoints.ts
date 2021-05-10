import { Request, Response } from "express";
import { getManager } from "typeorm";
import { StructuralPoint } from "../entity/StructuralPoint"

export async function listStructuralPoints(request: Request, response: Response) {

    const points = await getManager()
        .getRepository(StructuralPoint)
        .createQueryBuilder("structural_point")
        .where("structural_point.project = :id", { id:  request.params.id })
        .orderBy("structural_point.name", "ASC")
        .getMany();

    // const points = getManager().getRepository(StructuralPoint).find({ where: { project: 1 } });

    response.send(points);
}
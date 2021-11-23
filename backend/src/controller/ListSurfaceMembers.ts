import { Request, Response } from "express";
import { getManager } from "typeorm";
import { SurfaceMember } from "../entity/SurfaceMember";

export async function listSurfaceMembers(request: Request, response: Response) {

    const values = await getManager()
        .getRepository(SurfaceMember)
        .createQueryBuilder("obj")
        .leftJoinAndSelect('obj.material', 'notused1')
        .leftJoinAndSelect('obj.structuralPoints', 'notused2')
        .where("obj.project = :id", { id: request.params.id })
        .getMany();

    response.send(values);
}
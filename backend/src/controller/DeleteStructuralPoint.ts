import { Request, Response } from "express";
import { getManager } from "typeorm";
import { StructuralPoint } from "../entity/StructuralPoint"

export async function deleteStructuralPoint(request: Request, response: Response) {

    const res = await getManager().getRepository(StructuralPoint).delete(request.params.id)
}

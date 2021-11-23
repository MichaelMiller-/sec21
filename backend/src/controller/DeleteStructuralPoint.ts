import { Request, Response } from "express";
import { getManager } from "typeorm";
import { StructuralPoint } from "../entity/StructuralPoint"
import Result from "../dto/Result";

export async function deleteStructuralPoint(request: Request, response: Response) {

    let result = new Result<null>();

    await getManager().getRepository(StructuralPoint)
        .delete(request.params.id)
        .catch((ex) => {
            const msg = ex
            result.success = false
            result.message = msg.detail
        });

    response.send(result);
}

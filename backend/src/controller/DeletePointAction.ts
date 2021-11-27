import {Request, Response} from "express";
import {getManager} from "typeorm";
import Result from "../dto/Result";
import {PointAction} from "../entity/PointAction";
import logger from "../logging/Logger";

export async function deletePointAction(request: Request, response: Response) {

    let result = new Result<null>();

    await getManager().getRepository(PointAction)
        .delete(request.params.id)
        .catch((ex) => {
            logger.error(ex);
            result.success = false
            result.message = ex.detail
        });

    response.send(result);
}

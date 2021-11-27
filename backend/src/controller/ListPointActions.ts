import {Request, Response} from "express";
import {getManager} from "typeorm";
import {PointAction} from "../entity/PointAction";

export async function listPointActions(request: Request, response: Response) {

    const values = await getManager()
        .getRepository(PointAction)
        .createQueryBuilder("obj")
        .leftJoinAndSelect('obj.referencePoint', 'notused')
        .leftJoinAndSelect('obj.loadcase', 'lc')
        .where("lc.project = :id", {id: request.params.id})
        .getMany();

    response.send(values);
}
import { Request, Response } from "express";
import { getManager } from "typeorm";
import { PointSupport } from "../entity/PointSupport";

export async function listPointSupports(request: Request, response: Response) {

   const values = await getManager()
      .getRepository(PointSupport)
      .createQueryBuilder("obj")
      .leftJoinAndSelect('obj.referencePoint', 'refpt')
      .where("refpt.project = :id", { id: request.params.id })
      .getMany();

   response.send(values);
}
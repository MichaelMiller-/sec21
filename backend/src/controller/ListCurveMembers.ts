import { Request, Response } from "express";
import { getManager } from "typeorm";
import { CurveMember } from "../entity/CurveMember";

export async function listCurveMembers(request: Request, response: Response) {

   const values = await getManager()
      .getRepository(CurveMember)
      .createQueryBuilder("obj")
      .where("obj.project = :id", { id: request.params.id })
      .getMany();

   response.send(values);
}
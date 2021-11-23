import { Request, Response } from "express";
import { getManager } from "typeorm";
import { CurveMember } from "../entity/CurveMember";

export async function listCurveMembers(request: Request, response: Response) {

   const values = await getManager()
      .getRepository(CurveMember)
      .createQueryBuilder("obj")
      .leftJoinAndSelect('obj.beginNode', 'notused1')
      .leftJoinAndSelect('obj.endNode', 'notused2')
      .leftJoinAndSelect('obj.crossSection', 'notused3')
      .where("obj.project = :id", { id: request.params.id })
      .getMany();

   response.send(values);
}
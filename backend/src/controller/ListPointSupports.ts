import { Request, Response } from "express";
import { getManager } from "typeorm";
import { CurveMember } from "../entity/CurveMember";
import { PointSupport } from "../entity/PointSupport";

export async function listPointSupports(request: Request, response: Response) {

   const values = await getManager()
      .getRepository(PointSupport)
      .createQueryBuilder("obj")
      .leftJoinAndSelect('obj.reference_point', 'structural_point')
      // .where("obj.reference_point.project = :id", { id: request.params.id })
      .getMany();

   console.log("values:", values);
   response.send(values);
}
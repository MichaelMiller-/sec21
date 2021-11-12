import { Request, Response } from "express";
import { getManager } from "typeorm";
import { CrossSection } from "../entity/CrossSection";

export async function listCrossSections(request: Request, response: Response) {

   const values = await getManager()
      .getRepository(CrossSection)
      .createQueryBuilder("obj")
      .leftJoinAndSelect('obj.material', 'notused1')
      .where("obj.project = :id", { id: request.params.id })
      .orderBy("obj.name", "ASC")
      .getMany();

   response.send(values);
}
import { Request, Response } from "express";
import { getManager } from "typeorm";
import { CrossSection } from "../entity/CrossSection";

export async function listCrossSections(request: Request, response: Response) {

   const values = await getManager()
      .getRepository(CrossSection)
      .createQueryBuilder("crosssection")
      .where("crosssection.project = :id", { id: request.params.id })
      .orderBy("crosssection.name", "ASC")
      .getMany();

   response.send(values);
}
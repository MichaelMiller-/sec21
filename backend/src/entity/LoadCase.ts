import { Entity, PrimaryGeneratedColumn, Column, ManyToOne, Generated } from "typeorm";
import { Project } from "./Project";

export enum ActionType {
   PERMANENT = "Permanent",
   VARIABLE = "Variable",
   ACCIDENTAL = "Accidental"
}

export enum LoadType {
   SELFWEIGHT = "SelfWeight",
   OTHERS = "Others",
   PRESTRESS = "Prestress",
   DYNAMIC = "Dynamic",
   STATIC = "Static",
   TEMPERATURE = "Temperature",
   WIND = "Wind",
   SNOW = "Snow",
   MAINTENANCE = "Maintenance",
   FIRE = "Fire",
   MOVING = "Moving",
   SEISMIC = "Seismic",
   STANDARD = "Standard"
}

@Entity()
export class LoadCase {

   @PrimaryGeneratedColumn()
   id: number;

   @Column()
   name: string;

   @Column()
   @Generated("uuid")
   uuid: string;

   @Column()
   description: string;

   @Column({
      type: "enum",
      enum: ActionType,
      default: ActionType.PERMANENT
   })
   actionType: ActionType

   @Column({
      type: "enum",
      enum: LoadType,
      default: LoadType.STANDARD
   })
   type: LoadType

   @ManyToOne(type => Project, project => project.loadCases)
   project: Project;
}
import { Entity, PrimaryGeneratedColumn, Column, ManyToOne, Generated } from "typeorm";
import { CrossSection } from "./CrossSection";
import { Project } from "./Project";
import { StructuralPoint } from "./StructuralPoint";

export enum SystemLine {
   CENTRE = "centre",
   TOP = "top",
   BOTTOM = "buttom",
   LEFT = "left",
   RIGHT = "right",
   TOPLEFT = "top_left",
   TOPRIGHT = "top_right",
   BOTTOMLEFT = "buttom_left",
   BOTTOMRIGHT = "buttom_right"
}

@Entity()
export class CurveMember {

   @PrimaryGeneratedColumn()
   id: number;

   @Column()
   name: string;

   @Column()
   @Generated("uuid")
   uuid: string;

   @ManyToOne(type => CrossSection, cs => cs.id)
   crossSection: CrossSection;

   @ManyToOne(type => StructuralPoint, pt => pt.id)
   beginNode: StructuralPoint;

   @ManyToOne(type => StructuralPoint, pt => pt.id)
   endNode: StructuralPoint;

   @Column({
      type: "enum",
      enum: SystemLine,
      default: SystemLine.CENTRE
   })
   systemLine: SystemLine;

   @ManyToOne(type => Project, project => project.crossSections)
   project: Project;
}
import { Entity, PrimaryGeneratedColumn, Column, Generated, JoinColumn, OneToOne } from "typeorm";
import { StructuralPoint } from "./StructuralPoint";

export enum SupportType {
   FIXED = "fixed",
   HINGED = "hinged",
   SLIDING = "sliding",
   CUSTOM = "custom"
}

export enum SupportTranslation {
   RIGID = "rigid",
   FREE = "free",
   FLEXIBLE = "flexible",
   COMPRESSION_ONLY = "compression_only",
   TENSION__ONLY = "tension_only",
   //  "Flexible compression only",
   // "Flexible tension only",
   NON_LINEAR = "non_linear"
}

export enum SupportRotationalStiffness {
   FREE = "free",
   RIGID = "rigid",
   FLEXIBLE = "flexible",
   NON_LINEAR = "non_linear"
}

@Entity()
export class PointSupport {

   @PrimaryGeneratedColumn()
   id: number;

   @Column()
   @Generated("uuid")
   uuid: string;

   @Column()
   name: string;

   @OneToOne(type => StructuralPoint)
   @JoinColumn()
   referencePoint: StructuralPoint;

   @Column({
      type: "enum",
      enum: SupportType,
      default: SupportType.FIXED
   })
   type: SupportType;

   @Column({
      type: "enum",
      enum: SupportTranslation,
      default: SupportTranslation.FREE
   })
   ux: SupportTranslation;

   @Column({
      type: "enum",
      enum: SupportTranslation,
      default: SupportTranslation.FREE
   })
   uy: SupportTranslation;

   @Column({
      type: "enum",
      enum: SupportTranslation,
      default: SupportTranslation.FREE
   })
   uz: SupportTranslation;

   @Column({
      type: "enum",
      enum: SupportRotationalStiffness,
      default: SupportRotationalStiffness.FREE
   })
   fix: SupportRotationalStiffness;

   @Column({
      type: "enum",
      enum: SupportRotationalStiffness,
      default: SupportRotationalStiffness.FREE
   })
   fiy: SupportRotationalStiffness;

   @Column({
      type: "enum",
      enum: SupportRotationalStiffness,
      default: SupportRotationalStiffness.FREE
   })
   fiz: SupportRotationalStiffness;

   @Column("real")
   stiffnessX: number;

   @Column("real")
   stiffnessY: number;

   @Column("real")
   stiffnessZ: number;

   @Column("real")
   stiffnessFix: number;

   @Column("real")
   stiffnessFiy: number;

   @Column("real")
   stiffnessFiz: number;
}